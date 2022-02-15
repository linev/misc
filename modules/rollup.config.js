import json from "@rollup/plugin-json";
import resolve from "@rollup/plugin-node-resolve";
// import commonjs from "@rollup/plugin-commonjs";
// import babel from '@rollup/plugin-babel';
// import ascii from "rollup-plugin-ascii";
// import async from "rollup-plugin-async";

const config0 = {
  plugins: [
//    async(),
/*    babel({
        babelrc: false,
        exclude: 'node_modules/**',
        presets: [
          [
            '@babel/preset-env',
            {
              corejs: 3,
              modules: false,
              useBuiltIns: 'usage',
              targets: {
                ie: '11',
              },
            },
          ],
        ],
      }),
*/
    resolve(),
//    commonjs(),
    json(),
//    ascii()
  ],
  onwarn(message, warn) {
    if (message.code === "CIRCULAR_DEPENDENCY") return;
    warn(message);
  }
};


const config_module_base = {
   ...config0,
  input: "scripts/module_base.js",
  output: {
    file: `build/module_base.js`,
    name: "module_base",
    format: "umd",
    indent: false,
    extend: true,
    banner: `// nothing special`
  }
};

const config_module1 = {
   ...config0,
  input: "scripts/module1.js",
  output: {
    dir: `build/`,
    name: "module1",
    format: "es",
    indent: false,
    extend: true,
    banner: `// nothing special`
  }
};

const config_module2 = {
   ...config0,
  input: "scripts/module2.js",
  output: {
    dir: `build/`,
    name: "module2",
    format: "es",
    indent: false,
    extend: true,
    banner: `// nothing special`
  }
};

export default [
  config_module_base, config_module1, config_module2

];
