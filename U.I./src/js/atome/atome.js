const atomeJS = Object.assign(atomeAnimate,atomeCommunication,atomeDrag,atomeDrop,atomeFile,atomeSort, atomeTime, atomeVideo);
// This const is used to store and get all atome created canvas by id
const Atome_canvas={}
// // class AtomeDrag {
// //     constructor() {
// //     }
// //
// //     drag(atome_drag_id) {
// //         // target elements with the "draggable" class
// //         self.current_obj = Opal.Utilities.$grab(atome_drag_id)
// //         interact('.'+atome_drag_id)
// //             .draggable({
// //                 // enable inertial throwing
// //                 startAxis: 'x',
// //                 lockAxis: 'x',
// //                 // lockAxis: ''+lock_axis,
// //                 inertia: true,
// //                 // keep the element within the area of it's parent
// //                 modifiers: [
// //                     interact.modifiers.restrictRect({
// //                         restriction: 'parent',
// //                         endOnly: true
// //                     })
// //                 ],
// //                 // enable autoScroll
// //                 autoScroll: true,
// //
// //                 listeners: {
// //                     // call this function on every dragmove event
// //
// //                     move: dragMoveListener,
// //                     start(event) {
// // //TODO:  optimise this passing the proc to the drag callback
// //                         // lets get the current atome Object
// //                         // self.current_obj = Opal.Utilities.$grab(atome_drag_id)
// //                         // now get the grab proc
// //                         self.proc_meth = current_obj.bloc
// //                     },
// //                     // call this function on every dragend event
// //                     end(event) {
// //                     }
// //                 }
// //             })
// //
// //         function dragMoveListener(event) {
// //             const target = event.target
// //             // the code below can be conditioned to receive the drag event without moving the object
// //             // keep the dragged position in the data-x/data-y attributes
// //             const x = (parseFloat(target.getAttribute('data-x')) || 0) + event.dx;
// //             const y = (parseFloat(target.getAttribute('data-y')) || 0) + event.dy
// //             // translate the element
// //             target.style.transform = 'translate(' + x + 'px, ' + y + 'px)'
// //             // update the position attributes
// //             target.setAttribute('data-x', x)
// //             target.setAttribute('data-y', y)
// //             // CallBack here
// //             self.current_obj.$dragCallback(event.pageX, event.pageY, event.rect.left, event.rect.top, self.current_obj, self.proc_meth);
// //         }
// //     }
// //
// // }
//
// // Usage:
// // let atomeDrag = new AtomeDrag();
// // atomeDrag.drag('atome_id', 'options');
// // const Atomeanimation = {}
//
//
//
// // TODO: put in a class
//
//
// const atome = {
//     jsSchedule: function (years, months, days, hours, minutes, seconds, atome, proc) {
//         const now = new Date();
//         const formatDate = new Date(years, months - 1, days, hours, minutes, seconds);
//         const diffTime = Math.abs(formatDate - now);
//         setTimeout(function () {
//             atome.$schedule_callback(proc);
//         }, diffTime);
//     },
//     jsReader: function (file, atome, proc) {
//         fetch('medias/' + file)
//             .then(response => response.text())
//             .then(text => atome.$read_callback(text, proc))
//     },
//     js_Fullscreen: function (atome_id){
//         let elem = document.getElementById(atome_id)
//         if (elem.requestFullscreen) {
//             elem.requestFullscreen();
//         } else if (elem.webkitRequestFullscreen) { /* Safari */
//             elem.webkitEnterFullscreen();
//         } else if (elem.msRequestFullscreen) { /* IE11 */
//             elem.msRequestFullscreen();
//         }
//
//     }
//
// }


///////////////////////////////////////////////////////////
// //// example ruby equivalent in js ///
// Opal.queue(function (Opal) {/* Generated by Opal 1.6.0 */
//     const $klass = Opal.klass, $defs = Opal.defs, $def = Opal.def;
// // The line below create the AtomicJS cons to simplify access to js object from ruby
//
//     // var $const_set = Opal.const_set, $nesting = [], nil = Opal.nil;
//     // return $const_set($nesting[0], 'AtomicJS', atome)
//
//     (function ($base, $super) {
//         const self = $klass($base, $super, 'AtomeJS');
//         $def(self, '$initialize', function $$initialize(val) {
//             // Init code below
//         }, 1);
//
//         $defs(self, '$check_this', function $$check_this(val) {
//             // getter code below
//             // alert ('Suoer cool!!'+val)
//             // alert(atomeJS)
//             return (val);
//         }, 0);
//
//         $def(self, '$way', function $way() {
//             // getter code below
//             return (self.my_var);
//         }, 0);
//
//         return $def(self, '$way=', function $way(val) {
//             //self method example
//             return (self.my_var = val);
//         }, 0);
//
//
//     })();
//
//     // new class below
//     return (function ($base, $super) {
//         const self = $klass($base, $super, 'Atome');
//
//         $defs(self, '$verif', function $$verif(val) {
//             //self method example
//             return (val);
//         }, 0);
//
//         $def(self, '$new_method', function $way() {
//             // getter code below
//             return (self.new_method);
//         }, 0);
//
//
//         return $def(self, '$new_method=', function $way(val) {
//             //setter code below
//             return (self.atome = val);
//         }, 0);
//
//     })();
// });



