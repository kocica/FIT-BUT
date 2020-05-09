"use strict";
async function onTransferSubmit() {
    // TODO: Some checks etc.? (e.g. accounts existance, src != dst, amount > 0, ...)
    $("#submitHelperForm").submit();
}
async function onBankIdentifierInputChangeSrc() {
    let inputField = $("#accountIdSrcInput")[0];
    if (inputField.value.length < 3) {
        return;
    }
    let req = {
        prefixId: inputField.value
    };
    let result = await new Promise(resolve => $.post("/api/getBankAccountListPrefix", req, resolve));
    let datalist = $("#accountListSrc");
    datalist.empty();
    result.forEach(value => datalist.append($(`<option value="${value.identifier}" data-id="${value.number}">`)));
}
async function onBankIdentifierInputChangeDst() {
    let inputField = $("#accountIdDstInput")[0];
    if (inputField.value.length < 3) {
        return;
    }
    let req = {
        prefixId: inputField.value
    };
    let result = await new Promise(resolve => $.post("/api/getBankAccountListPrefix", req, resolve));
    let datalist = $("#accountListDst");
    datalist.empty();
    result.forEach(value => datalist.append($(`<option value="${value.identifier}" data-id="${value.number}">`)));
}
//# sourceMappingURL=transfer.js.map