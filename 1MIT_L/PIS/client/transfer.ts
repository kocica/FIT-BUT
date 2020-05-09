async function onTransferSubmit()
{
    // TODO: Some checks etc.? (e.g. accounts existance, src != dst, amount > 0, ...)

	$("#submitHelperForm").submit()
}

async function onBankIdentifierInputChangeSrc()
{
	let inputField = $("#accountIdSrcInput")[0] as HTMLInputElement
	if(inputField.value.length < 3)
	{
		return
	}
	let req: GetBankAccountListPrefixRequest = {
		prefixId: inputField.value
	}
	let result = await new Promise(resolve => $.post("/api/getBankAccountListPrefix", req, resolve)) as GetBankAccountListResponse
	let datalist = $("#accountListSrc")
	datalist.empty()
	result.forEach(value => datalist.append($(`<option value="${value.identifier}" data-id="${value.number}">`)))
}

async function onBankIdentifierInputChangeDst()
{
	let inputField = $("#accountIdDstInput")[0] as HTMLInputElement
	if(inputField.value.length < 3)
	{
		return
	}
	let req: GetBankAccountListPrefixRequest = {
		prefixId: inputField.value
	}
	let result = await new Promise(resolve => $.post("/api/getBankAccountListPrefix", req, resolve)) as GetBankAccountListResponse
	let datalist = $("#accountListDst")
	datalist.empty()
	result.forEach(value => datalist.append($(`<option value="${value.identifier}" data-id="${value.number}">`)))
}