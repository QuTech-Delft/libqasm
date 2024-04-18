import * as wrapper from '../build/Release/emscripten/cqasm_emscripten.mjs';
import puppeteer from 'puppeteer';

process.on("unhandledRejection", error => {
    console.error("unhandledRejection", error, "\n");
});

(async () => {
    // Launch browser in headful mode
    const browser = await puppeteer.launch({ headless: true });
    const page = await browser.newPage();

    // Enable logging
    page.on('console', message => console.log(`Page log: ${message.text()}`));
    page.on('error', error => console.error(`Page error: ${error}`));
    page.on('pageerror', pageError => console.error(`Page error: ${pageError}`));

    wrapper.default().then(function(result) {
        var cqasm = new result["EmscriptenWrapper"]()
        var ret_code = 0

        console.log("Hola from JavaScript!")

        var cqasm = new result["EmscriptenWrapper"]();
        try {
            var output = cqasm.get_version()
            var expected_output = "0.5.2"
            console.log("\nThe version of libqasm compiled with emscripten is:", output);
            if (output !== expected_output) {
                console.log("\tExpected output:", expected_output)
                ret_code |= 1
            }
        } catch (e) {
            console.log(e.stack);
        }

        if (ret_code === 0) {
            console.log("\nOK\n")
        } else {
            console.log("\nError: returning", ret_code.toString(16), "\n")
        }
        return process.exit(ret_code)
    });
})();
