console.log('load module2.js');

import jsroot from './core.js';

export default jsroot.require(['module_base']).then(res2 => {

   console.log('res2', res2[0]);

   return { name: "module2" };
});
