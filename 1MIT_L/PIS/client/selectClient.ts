/// <reference path="../commonTypes/getClientList.d.ts" />

//example of clientside JS generation

function isApiClient(obj: any): obj is ApiClient
{
	return typeof obj == "object" && typeof obj.id == "number" && typeof obj.name == "string" &&
		(obj.birthdate instanceof Date || (typeof obj.birthdate == "string" && !!Date.parse(obj.birthdate)))
}

function isGetClientListResponse(obj: any): obj is GetClientListResponse
{
	return obj instanceof Array && obj.every(val => isApiClient(val))
}

async function onClientNameInputChange()
{
	let inputField = $("#clientNameInput")[0] as HTMLInputElement
	if(inputField.value.length < 3)
	{
		//at least some selection
		return
	}
	let req: GetClientListRequest = {
		namePrefix: inputField.value
	}
	let result = await new Promise(resolve => $.post("/api/getClientList", req, resolve))
	if(!isGetClientListResponse(result))
	{
		return
	}
	let datalist = $("#clientList")
	//empty datalist
	datalist.empty()
	//add new elements
	//TODO: select by date
	result.forEach(value => datalist.append($(`<option value="${value.name}" data-id="${value.id}">`)))
}

async function onClientSubmit()
{
	let inputField = $("#clientNameInput")[0] as HTMLInputElement

	if( inputField.value.length < 5 )
    {
        document.getElementById('errorMsg')!.innerHTML="Login is too short. Login has to be of length at least 5 characters.";
        return; // Abort, cannot proceed
	}

	let selectedId = $(`#clientList option[value="${inputField.value}"]`).attr("data-id")
	if(!selectedId)
	{
		document.getElementById('errorMsg')!.innerHTML="Client with given login does not exist in the system.";
		return
	}
	$("#clientIdInput").val(selectedId)
	$("#submitHelperForm").submit()
}