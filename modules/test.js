import jsroot from './scripts/core.js';

console.log('version', jsroot.version);

let arr = await jsroot.require(['module1', 'module2']);

console.log('arr', arr[0], arr[1]);
