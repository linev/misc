# Draft of special project to build Component-preload.js for ROOT canvas

    npm install
    node_modules/@ui5/cli/bin/ui5.js build preload --exclude-task='*' --include-task=generateComponentPreload
    cp dist/Component-preload.js ../../webgui/ui5/canv


## Useful links

    https://stackoverflow.com/questions/45061298/component-preload-js-generation
    
    https://openui5.hana.ondemand.com/1.32.4/docs/guide/be0cf40f61184b358b5faedaec98b2da.html
    
    https://github.com/SAP/openui5-sample-app
    
    https://sap.github.io/ui5-tooling/pages/Configuration/
