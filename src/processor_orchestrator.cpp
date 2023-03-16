#include "processor_orchestrator.hpp"

#include <set>

#include "application.hpp"

#include "modules/audio_input.hpp"
#include "modules/audio_output.hpp"
#include "modules/envelope.hpp"
#include "modules/gain.hpp"
#include "modules/high_pass.hpp"
#include "modules/low_pass.hpp"
#include "modules/midi_input.hpp"
#include "modules/mixer.hpp"
#include "modules/multiplier.hpp"
#include "modules/noise.hpp"
#include "modules/oscillator.hpp"
#include "modules/recorder.hpp"
#include "modules/sample.hpp"

using namespace std;

using namespace nlohmann;
using namespace live::tritone::vie::processor::module;

namespace live::tritone::vie
{
	processor_orchestrator::processor_orchestrator() : nb_modules_(0),
		processor_modules_{}, nb_midi_input_modules_(0),
		sources_midi_input_modules_{},
		nb_audio_input_modules_(0),
		sources_audio_input_modules_{},
		processing_setup_(),
		bypass_(false)
	{
	}

	void processor_orchestrator::initialize()
	{
		terminate();
	}

	const std::shared_ptr<processor_module> processor_orchestrator::add_processor_module(json processor_definition)
	{
		std::shared_ptr<processor_module> processor;
		const std::string type = processor_definition["type"];

		if (type == "midi-in")
		{
			processor = std::make_shared<midi_input>(processor_definition);
		}
		else if (type == "audio-in")
		{
			processor = std::make_shared<audio_input>(processor_definition);
		}
		else if (type == "oscillator")
		{
			processor = std::make_shared<oscillator>(processor_definition);
		}
		else if (type == "envelope")
		{
			processor = std::make_shared<envelope>(processor_definition);
		}
		else if (type == "multiplier")
		{
			processor = std::make_shared<multiplier>(processor_definition);
		}
		else if (type == "mixer")
		{
			processor = std::make_shared<mixer>(processor_definition);
		}
		else if (type == "sample")
		{
			processor = std::make_shared<processor::module::sample>(processor_definition);
		}
		else if (type == "audio-out")
		{
			processor = std::make_shared<audio_output>(processor_definition);
		}
		else if (type == "low-pass")
		{
			processor = std::make_shared<low_pass>(processor_definition);
		}
		else if (type == "high-pass")
		{
			processor = std::make_shared<high_pass>(processor_definition);
		}
		else if (type == "gain")
		{
			processor = std::make_shared<gain>(processor_definition);
		}
		else if (type == "recorder")
		{
			processor = std::make_shared<recorder>(processor_definition);
		}

		processor->initialize(processor_definition);

#ifdef VIE_DEBUG
		debugLogger.write("Added processor: " + processor->get_name());
#endif
		add_processor_module(processor);

		return processor;
	}

	void processor_orchestrator::add_processor_module(std::shared_ptr<processor_module> processor)
	{
		if (processor->get_type() == processor_module_type::event_input)
		{
			sources_midi_input_modules_[nb_midi_input_modules_] = std::dynamic_pointer_cast<midi_input>(processor);
			nb_midi_input_modules_++;
		}
		if (processor->get_type() == processor_module_type::audio_input)
		{
			sources_audio_input_modules_[nb_audio_input_modules_] = std::dynamic_pointer_cast<audio_input>(processor);
			nb_audio_input_modules_++;
		}

		processor_modules_[nb_modules_] = processor;

		nb_modules_++;
	}

	void processor_orchestrator::delete_processor(uint_fast8_t id)
	{
		auto module_to_delete = processor_modules_[id];

		//Remove links to the processor.
		for (int i = 0; i < nb_modules_ - 1; i++)
		{
			auto current_module = processor_modules_[i];
			current_module->unlink(module_to_delete);
		}

		//Remove the requested processor.
		for (int i = id; i < nb_modules_ - 1; i++)
		{
			processor_modules_[i] = processor_modules_[i + 1];
		}

		nb_modules_--;
	}

	uint_fast8_t processor_orchestrator::get_nb_processors()
	{
		return nb_modules_;
	}

	void processor_orchestrator::clear()
	{
		nb_modules_ = 0;
	}

	std::shared_ptr<processor_module> processor_orchestrator::get_processor(int id)
	{
		return processor_modules_[id];
	}

	std::array<std::shared_ptr<processor_module>, 128> processor_orchestrator::get_processor_modules(uint_fast8_t& nb_modules /*out*/)
	{
		nb_modules = nb_modules_;
		return processor_modules_;
	}

	uint16_t processor_orchestrator::link_modules(uint_fast8_t source_module_id, uint_fast16_t source_slot_id, uint_fast8_t target_module_id, uint_fast16_t target_slot_id)
	{
		return processor_modules_[source_module_id]->link(source_slot_id, processor_modules_[target_module_id], target_slot_id);
	}

	void processor_orchestrator::unlink_modules(uint_fast8_t source_module_id, uint_fast16_t source_slot_id, uint_fast8_t target_module_id, uint_fast16_t target_slot_id)
	{
		processor_modules_[source_module_id]->unlink(source_slot_id, processor_modules_[target_module_id], target_slot_id);
	}

	void processor_orchestrator::enable_modules_link(uint_fast8_t source_module_id, uint_fast16_t source_slot_id, uint_fast8_t target_module_id, uint_fast16_t target_slot_id)
	{
		processor_modules_[source_module_id]->enable_link(source_slot_id, processor_modules_[target_module_id], target_slot_id);
	}

	void processor_orchestrator::disable_modules_link(uint_fast8_t source_module_id, uint_fast16_t source_slot_id, uint_fast8_t target_module_id, uint_fast16_t target_slot_id)
	{
		processor_modules_[source_module_id]->disable_link(source_slot_id, processor_modules_[target_module_id], target_slot_id);
	}

	void processor_orchestrator::terminate()
	{
		//Release processor modules.
		for (int i = 0; i < nb_modules_; i++)
		{
			//orchestrator is not owner of module.
			//const auto* module = processor_modules_[i];
			//delete module;
			processor_modules_[i] = nullptr;
		}

		nb_modules_ = 0;
		nb_midi_input_modules_ = 0;
		nb_audio_input_modules_ = 0;
	}

	void processor_orchestrator::setup_processing(const processing_setup& setup)
	{
		processing_setup_.sample_rate = setup.sample_rate;
		processing_setup_.processing_mode = setup.processing_mode;
		processing_setup_.max_samples_per_block = setup.max_samples_per_block;
		processing_setup_.sample_size = setup.sample_size;

		for (int i = 0; i < nb_modules_; i++)
		{
			auto module = processor_modules_[i];
			module->set_sample_rate(processing_setup_.sample_rate);
		}
	}

	void processor_orchestrator::process_input_event(event& event) const
	{
		std::shared_ptr<midi_input> midi_input_module = get_midi_input_module_for_event(event);
		switch (event.type)
		{
		case event::type::note_on:
		{
			note_event& note_on_event = event.core_event.note_on;
			midi_input_module->note_on(note_on_event);
		}
		break;
		case event::type::note_off:
		{
			note_event& note_off_event = event.core_event.note_off;
			midi_input_module->note_off(note_off_event);
		}
		break;
		case event::type::data_event:
		{
			note_event& note_off_event = event.core_event.note_off;
			midi_input_module->note_off(note_off_event);
		}
		break;
		case event::type::poly_pressure_event:
		case event::type::note_expression_value_event:
		case event::type::note_expression_text_event:
		case event::type::chord_event:
		case event::type::scale_event:
		case event::type::legacy_midi_cc_out_event:
			break;
		}
	}

	void processor_orchestrator::process_input_audio(audio_bus_buffers* buffer, int32_t buffer_id) const
	{
		auto audio_input = get_audio_input_module_for_buffer(buffer_id);
		audio_input->set_buffer(buffer);
	}

	void processor_orchestrator::process(output_process_data& output_process_data)
	{
		if (!bypass_) {
			for (int i = 0; i < nb_modules_; i++)
			{
				auto module = processor_modules_[i];
				module->preprocess();
			}
			for (int i = 0; i < nb_midi_input_modules_; i++)
			{
				auto midi_input_module = sources_midi_input_modules_[i];
				//TODO: Multi-thread call to this method.
				process(std::dynamic_pointer_cast<processor_module>(midi_input_module), output_process_data);
			}
			for (int i = 0; i < nb_audio_input_modules_; i++)
			{
				auto audio_input_module = sources_audio_input_modules_[i];
				//TODO: Multi-thread call to this method.
				process(std::dynamic_pointer_cast<processor_module>(audio_input_module), output_process_data);
			}
		}
	}

	void processor_orchestrator::process(std::shared_ptr<processor_module> source_module,
		output_process_data& output_process_data)
	{
		// If all input of source module are not filled in by parent module, we cannot process it.
		// We need to wait for each parent to process before this module.
		// The last parent of this module triggers the process.
		if (source_module->can_process())
		{
			//Process source module
			source_module->process(output_process_data);

			//Get all children of source modules
			uint_fast8_t nb_module_links;
			std::array<std::shared_ptr<module_link>, 32>& module_links = source_module->get_modules_links(nb_module_links);

			//Process all children.
			for (int i = 0; i < nb_module_links; i++)
			{
				const auto [enabled, source_slot_id, target_module, target_slot_id] = *(module_links[i]);

				std::array<module_output*, 32> source_output_values;

				//Because module has been processed, we can get its output values.
				uint_fast8_t nb_outputs = source_module->get_output_values(
					source_slot_id, source_output_values);

				//And then pass output values to input values of next module.
				target_module->set_input_values(target_slot_id, source_output_values, nb_outputs);

				// And process next module.
				process(target_module, output_process_data);
			}
		}
	}

	void processor_orchestrator::parameter_changed(const unsigned long parameter_id, long sample_offset, double parameter_value)
	{
		unsigned int component_id = parameter_id >> 16;
		unsigned int component_parameter_id = parameter_id & 0xffff;

		float_module_output input(0, parameter_value);
		std::array<module_output*, 32> input_values = { &input };

		processor_modules_[component_id]->set_input_values(component_parameter_id, input_values, 1);
	}

	std::shared_ptr<midi_input> processor_orchestrator::get_midi_input_module_for_event(const event& event) const
	{
		return sources_midi_input_modules_[event.bus_index];
	}

	std::shared_ptr<audio_input> processor_orchestrator::get_audio_input_module_for_buffer(int32_t buffer_id) const
	{
		return sources_audio_input_modules_[buffer_id];
	}
} // namespace
