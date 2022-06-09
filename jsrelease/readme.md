# Steps for JSROOT release

1. Set version in package.json
2. Set version in modules/core.mjs
3. Run build `npm run build`
4. Commit changes
5. Create subversion branch when minor version changes, push to github
6. Create documentation `npm run doc`
7. Update `index.html` for website, copy on website
8. Call `make` into directory to copy all JSROOT files on website
9. Copy directory on website into release directory, change access flags `chmod -w -R <dirname>`
10. Change `package.json` in jstest, create correspondent branch and tags, push tags to github
11. Call `npm publish` in JSROOT to publish in npm
