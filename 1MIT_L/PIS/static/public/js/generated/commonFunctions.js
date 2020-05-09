"use strict";
function GetErrorText() {
    var _a;
    return (_a = new URL(window.location.href).searchParams.get("error")) !== null && _a !== void 0 ? _a : "";
}
$("#errorMsg")[0].innerText = GetErrorText();
//# sourceMappingURL=commonFunctions.js.map