/// <reference path="../commonTypes/getBankAccountList.d.ts" />

//example of clientside JS generation

function isApiBankAccount2(obj: any): obj is ApiBankAccount
{
	return typeof obj == "object" && typeof obj.number == "number" && typeof obj.identifier == "string" &&
           typeof obj.amount == "number" && typeof obj.ownerId == "number"
}

function isGetBankAccountListResponse2(obj: any): obj is GetBankAccountListResponse
{
	return obj instanceof Array && obj.every(val => isApiBankAccount2(val))
}

async function onBankIdentifierInputChange()
{
	let inputField = $("#accountIdInput")[0] as HTMLInputElement
	if(inputField.value.length < 3)
	{
		//at least some selection
		return
	}
	let req: GetBankAccountListPrefixRequest = {
		prefixId: inputField.value
	}
	let result = await new Promise(resolve => $.post("/api/getBankAccountListPrefix", req, resolve))
	if(!isGetBankAccountListResponse2(result))
	{
		return
	}
	let datalist = $("#accountList")
	//empty datalist
	datalist.empty()
	//add new elements
	//TODO: select by date
	result.forEach(value => datalist.append($(`<option value="${value.identifier}" data-id="${value.number}">`)))
}

function onPageLoad()
{
	const params = new URL(window.location.href).searchParams
	let err = params.get("error")
	if(!err)
	{
		return
	}
	document.body.innerHTML = `<p style="color: red">${err}</p>`+document.body.innerHTML
}
