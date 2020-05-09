"use strict";
/// <reference path="../commonTypes/getBankAccount.d.ts" />
/// <reference path="../commonTypes/getBankTransactionList.d.ts" />
/// <reference path="../commonTypes/getAuthorizedPersonList.d.ts" />
Object.defineProperty(exports, "__esModule", { value: true });
async function showBankAccountInfo() {
    // Get and display account infos
    var urlString = window.location.href;
    var url = new URL(urlString);
    var bankAccountNumber = url.searchParams.get("ban");
    let req = {
        number: parseInt(bankAccountNumber)
    };
    let result = await new Promise(resolve => $.post("/api/getBankAccount", req, resolve));
    //$("#accountNr").val(result.number);
    $("#accountId").val(result.identifier);
    $("#accountBalance").val(result.amount);
    // Get and display account owner name
    let req2 = {
        id: result.ownerId
    };
    let result2 = await new Promise(resolve => $.post("/api/getClientById", req2, resolve));
    $("#accountOwner").val(result2.name);
    // Get and display account INCOMING transactions
    let req3 = {
        destAcc: result.identifier
    };
    let result3 = await new Promise(resolve => $.post("/api/getBankTransactionListByDest", req3, resolve));
    var transactionInTab = document.getElementById("transactionInTab");
    result3.forEach(value => {
        let newRow = transactionInTab.insertRow(-1); // Append new record to the table
        let newTypeCell = newRow.insertCell(0);
        let newDateCell = newRow.insertCell(1);
        let newFromCell = newRow.insertCell(2);
        let newAmountCell = newRow.insertCell(3);
        let newTypeCellText = document.createTextNode(value.type);
        let newDateCellText = document.createTextNode(`${value.date}`); // value.date.toLocaleDateString() simply does not work...
        let newFromCellText = document.createTextNode(value.sourceAccount == null ? "---" : value.sourceAccount);
        let newAmountCellText = document.createTextNode(value.amount.toString());
        newTypeCell.appendChild(newTypeCellText);
        newDateCell.appendChild(newDateCellText);
        newFromCell.appendChild(newFromCellText);
        newAmountCell.appendChild(newAmountCellText);
    });
    // Get and display account OUTOING transactions
    let req4 = {
        srcAcc: result.identifier
    };
    let result4 = await new Promise(resolve => $.post("/api/getBankTransactionListBySrc", req4, resolve));
    var transactionOutTab = document.getElementById("transactionOutTab");
    result4.forEach(value => {
        let newRow = transactionOutTab.insertRow(-1); // Append new record to the table
        let newTypeCell = newRow.insertCell(0);
        let newDateCell = newRow.insertCell(1);
        let newToCell = newRow.insertCell(2);
        let newAmountCell = newRow.insertCell(3);
        let newTypeCellText = document.createTextNode(value.type);
        let newDateCellText = document.createTextNode(`${value.date}`); // value.date.toLocaleDateString() simply does not work...
        let newToCellText = document.createTextNode(value.destinationAccount == null ? "---" : value.destinationAccount);
        let newAmountCellText = document.createTextNode(value.amount.toString());
        newTypeCell.appendChild(newTypeCellText);
        newDateCell.appendChild(newDateCellText);
        newToCell.appendChild(newToCellText);
        newAmountCell.appendChild(newAmountCellText);
    });
    // Get and display account authorized clients
    let req5 = {
        accountId: result.number
    };
    let result5 = await new Promise(resolve => $.post("/api/getAuthorizedPersonList", req5, resolve));
    var authorizedClientsTab = document.getElementById("authorizedClientsTab");
    result5.forEach(value => {
        let newRow = authorizedClientsTab.insertRow(-1); // Append new record to the table
        let newNameCell = newRow.insertCell(0);
        let newBdayCell = newRow.insertCell(1);
        let newNameCellText = document.createTextNode(value.name);
        let newBdayCellText = document.createTextNode(`${value.birthdate}`); // value.birthdate.toLocaleDateString() simply does not work...
        newNameCell.appendChild(newNameCellText);
        newBdayCell.appendChild(newBdayCellText);
    });
}
//# sourceMappingURL=showAccount.js.map