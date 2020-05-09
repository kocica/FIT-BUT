"use strict";
async function onCreateAccountSubmit() {
    let inputField = $("#clientNameInput")[0];
    let selectedId = $(`#clientList option[value="${inputField.value}"]`).attr("data-id");
    if (!selectedId) {
        return;
    }
    $("#clientIdInput").val(selectedId);
    $("#submitHelperForm").submit();
}
//# sourceMappingURL=createAccount.js.map