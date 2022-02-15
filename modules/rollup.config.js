import json from "@rollup/plugin-json";
import nodeResolve from "@rollup/plugin-node-resolve";
import ascii from "rollup-plugin-ascii";

const config0 = {
  plugins: [
    nodeResolve(),
    json(),
    ascii()
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
    name: "core",
    format: "umd",
    indent: false,
    extend: true,
    banner: `// nothing special`
  }
};

export default [
  config_module_base
];
