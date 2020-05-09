/// <reference path="../commonTypes/getClient.d.ts" />
/// <reference path="../commonTypes/getEmployeeList.d.ts" />

function isGetClientResponse22(obj: any): obj is GetClientResponse
{
	return true
}

function isApiBankAccount22(obj: any): obj is ApiBankEmployee
{
	return typeof obj == "object" && typeof obj.login == "string" && typeof obj.isTerminated == "boolean"
}

function isGetBankAccountListResponse22(obj: any): obj is GetBankAccountListResponse22
{
	return obj instanceof Array && obj.every(val => isApiBankAccount22(val))
}

async function showClientInfo2()
{
	// Get client infos
	let req: GetClientRequest = {
		id: 0 // Not needed anymore as current clientID is taken from session context
	}

	//let result = await new Promise(resolve => $.post("/api/getClientFromSession", req, resolve))

	// if(!isGetClientResponse22(result))
	// {
	// 	return
	// }

	//$("#clientID").val(result.id)
	// $("#clientName").val(result.name)
	// $("#clientBday").val(result.birthdate.toString())

	// Get client bank accounts
	let req2: GetBankAccountListRequest2 = {}

	let result2 = await new Promise(resolve => $.get("/api/getEmployeeList", req2, resolve))

	if(!isGetBankAccountListResponse22(result2))
	{
		console.log("Hello world!");
		return
	}

	var accountTab: HTMLTableElement = <HTMLTableElement> document.getElementById("employeeTable");

	result2.forEach(value => {
		let newRow = accountTab.insertRow(-1);     // Append new record to the table

		let newIdCell = newRow.insertCell(0);      // Identifier cell
		let newBalanceCell = newRow.insertCell(1); // Balance cell
		let newButtonCell = newRow.insertCell(2);  // Button to show account cell

		let newIdText = document.createTextNode(value.login);
		let newBalanceText = document.createTextNode("enable");
		if(value.isTerminated== true)
		{
			newBalanceText = document.createTextNode("disable");
		}

		var newButton = document.createElement("input");
		newButton.type = "button";
		newButton.value = "Delete";
		if(value.isTerminated== true)
		{
			newButton.value = "Activate";
		}
		//newButton.value = "Show account";
		newButton.classList.add('buttonWarning');
		newButton.onclick = function()
		{
			//window.location.replace(`/static/html/employee/showAccount.html?ban=${value.login}`)
	// 		$.ajax({
 //     url: "/api/removeEmployee",
 //     type: "POST",
 //     data: {
 //         login: `${value.login}`,
 //     },
 //     success: function (response) {
 //         if (response.status == 200) {
	// 				   showClientInfo2();
	// 					 console.log("Hello world!");
 //
 //             //Replace current location from the history via history API
 //             //window.history.replaceState({}, 'foo', '/foo');
 //             //window.location.replace(`/`)';
 //             //$("#form-id").submit();//if you want to post something up
 //         }
 //     }
 // });

 		$.ajax({
     url: "/api/removeEmployee",
     type: "POST",
     data: {
         login: `${value.login}`,
     },
		 success: function() {
		location.reload();
}
 });
		};

		newIdCell.appendChild(newIdText);
		newBalanceCell.appendChild(newBalanceText);
		newButtonCell.appendChild(newButton);
	});
}
