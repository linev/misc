console.log('load module1.js');

import jsroot from './core.js';

export default jsroot.require(['module_base']).then(res1 => {

  console.log('res1', res1[0]);

   return { name: "module1" };
});
