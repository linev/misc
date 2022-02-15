console.log('load module2.js');

import jsroot from './core.js';

export default jsroot.require(['module_base']).then(async res2 => {

   let p2 = await import("../src/painter2.js");

   return { name: "module2", Painter2: p2.Painter2 };
});
