"use strict";
async function onAddAuthorizedPersonSubmit() {
    // Get client
    let inputField = $("#clientNameInput")[0];
    let selectedId = $(`#clientList option[value="${inputField.value}"]`).attr("data-id");
    if (!selectedId) {
        document.getElementById('errorMsg').innerHTML = "Client does not exist in the system";
        return;
    }
    $("#clientIdInput").val(selectedId);
    // Submit
    $("#submitHelperForm").submit();
}
//# sourceMappingURL=addAuthorizedPerson.js.map