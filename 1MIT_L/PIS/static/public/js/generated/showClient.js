"use strict";
/// <reference path="../commonTypes/getClient.d.ts" />
/// <reference path="../commonTypes/getBankAccountList.d.ts" />
function isGetClientResponse(obj) {
    return true;
}
function isApiBankAccount(obj) {
    return typeof obj == "object" && typeof obj.number == "number" && typeof obj.identifier == "string" &&
        typeof obj.amount == "number" && typeof obj.ownerId == "number";
}
function isGetBankAccountListResponse(obj) {
    return obj instanceof Array && obj.every(val => isApiBankAccount(val));
}
async function showClientInfo() {
    // Get client infos
    let req = {
        id: 0 // Not needed anymore as current clientID is taken from session context
    };
    let result = await new Promise(resolve => $.post("/api/getClientFromSession", req, resolve));
    if (!isGetClientResponse(result)) {
        return;
    }
    $("#clientID").val(result.id);
    $("#clientName").val(result.name);
    $("#clientBday").val(result.birthdate.toString());
    // Get client bank accounts
    let req2 = {
        clientId: result.id
    };
    let result2 = await new Promise(resolve => $.post("/api/getBankAccountList", req2, resolve));
    if (!isGetBankAccountListResponse(result2)) {
        return;
    }
    var accountTab = document.getElementById("accountTable");
    result2.forEach(value => {
        let newRow = accountTab.insertRow(-1); // Append new record to the table
        let newIdCell = newRow.insertCell(0); // Identifier cell
        let newBalanceCell = newRow.insertCell(1); // Balance cell
        let newBalanceCell2 = newRow.insertCell(2); // Balance cell
        let newButtonCell = newRow.insertCell(3); // Button to show account cell
        let newIdText = document.createTextNode(value.identifier);
        let newBalanceText = document.createTextNode(value.amount.toString());
        let newBalanceText2 = document.createTextNode("owner");
        if (value.ownerId.toString() != $("#clientID").val()) {
            newBalanceText2 = document.createTextNode("share");
        }
        var newButton = document.createElement("input");
        newButton.type = "button";
        newButton.value = "Show account";
        newButton.classList.add('buttonOk');
        newButton.onclick = function () {
            $.ajax({
                url: "/api/selectAccount",
                type: "POST",
                data: {
                    accountId: `${value.identifier}`,
                },
                success: function () {
                    window.location.assign(`/static/html/employee/showAccount.html?ban=${value.number}`);
                }
            });
            //window.location.replace(`/static/html/employee/showAccount.html?ban=${value.number}`)
        };
        newIdCell.appendChild(newIdText);
        newBalanceCell.appendChild(newBalanceText);
        newBalanceCell2.appendChild(newBalanceText2);
        newButtonCell.appendChild(newButton);
    });
}
//# sourceMappingURL=showClient.js.map