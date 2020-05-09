"use strict";
/// <reference path="../commonTypes/getClient.d.ts" />
function isGetCurrentClientResponse(obj) {
    return typeof obj == "object";
}
async function getCurrentClient() {
    let req = {
        id: 0 // Do not care about ID as we retrieve it from session.
    };
    let result = await new Promise(resolve => $.post("/api/getClientFromSession", req, resolve));
    if (!isGetCurrentClientResponse(result)) {
        return;
    }
    // Set current client
    $("#currentClient").val(result.name);
    // Set also current time
    $("#currentTime").val(new Date().toLocaleDateString() + " " + new Date().toLocaleTimeString());
}
//# sourceMappingURL=getCurrentClient.js.map