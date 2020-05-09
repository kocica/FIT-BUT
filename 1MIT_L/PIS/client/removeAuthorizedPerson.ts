async function onRemoveAuthorizedPersonSubmit()
{
	// Get client
	let inputField = $("#clientNameInput")[0] as HTMLInputElement
	let selectedId = $(`#clientList option[value="${inputField.value}"]`).attr("data-id")
	if(!selectedId)
	{
		document.getElementById('errorMsg')!.innerHTML="Client does not exist in the system"
		return
	}
	$("#clientIdInput").val(selectedId)

	// Submit
	$("#submitHelperForm").submit()
}