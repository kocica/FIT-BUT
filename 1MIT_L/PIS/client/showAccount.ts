/// <reference path="../commonTypes/getBankAccount.d.ts" />
/// <reference path="../commonTypes/getBankTransactionList.d.ts" />
/// <reference path="../commonTypes/getAuthorizedPersonList.d.ts" />

import { GetBankTransactionListByDestRequest, GetBankTransactionListBySrcRequest, GetBankTransactionListResponse } from "../commonTypes/getBankTransactionList";

async function showBankAccountInfo()
{
	// Get and display account infos
	var urlString = window.location.href;
	var url = new URL(urlString);
	var bankAccountNumber = url.searchParams.get("ban");
	let req: GetBankAccountRequest = {
		number: parseInt(bankAccountNumber!)
	}
	let result = await new Promise(resolve => $.post("/api/getBankAccount", req, resolve)) as GetBankAccountResponse
	//$("#accountNr").val(result.number);
	$("#accountId").val(result.identifier);
	$("#accountBalance").val(result.amount);




	// Get and display account owner name
	let req2: GetClientRequest = {
		id: result.ownerId
	}
	let result2 = await new Promise(resolve => $.post("/api/getClientById", req2, resolve)) as GetClientResponse
	$("#accountOwner").val(result2.name);




	// Get and display account INCOMING transactions
	let req3: GetBankTransactionListByDestRequest = {
		destAcc: result.identifier
	}
	let result3 = await new Promise(resolve => $.post("/api/getBankTransactionListByDest", req3, resolve)) as GetBankTransactionListResponse
	var transactionInTab: HTMLTableElement = <HTMLTableElement> document.getElementById("transactionInTab");

	result3.forEach(value => {
		let newRow = transactionInTab.insertRow(-1);     // Append new record to the table

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
	let req4: GetBankTransactionListBySrcRequest = {
		srcAcc: result.identifier
	}
	let result4 = await new Promise(resolve => $.post("/api/getBankTransactionListBySrc", req4, resolve)) as GetBankTransactionListResponse
	var transactionOutTab: HTMLTableElement = <HTMLTableElement> document.getElementById("transactionOutTab");

	result4.forEach(value => {
		let newRow = transactionOutTab.insertRow(-1);     // Append new record to the table

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
	let req5: GetAuthorizedClientListRequest = {
		accountId: result.number
	}
	let result5 = await new Promise(resolve => $.post("/api/getAuthorizedPersonList", req5, resolve)) as GetAuthorizedClientListResponse
	var authorizedClientsTab: HTMLTableElement = <HTMLTableElement> document.getElementById("authorizedClientsTab");

	result5.forEach(value => {
		let newRow = authorizedClientsTab.insertRow(-1);     // Append new record to the table

		let newNameCell = newRow.insertCell(0);
		let newBdayCell = newRow.insertCell(1);

		let newNameCellText = document.createTextNode(value.name);
		let newBdayCellText = document.createTextNode(`${value.birthdate}`); // value.birthdate.toLocaleDateString() simply does not work...

		newNameCell.appendChild(newNameCellText);
		newBdayCell.appendChild(newBdayCellText);
	});
}
