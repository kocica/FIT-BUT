

function GetErrorText()
{
	return new URL(window.location.href).searchParams.get("error") ?? ""
}

$("#errorMsg")[0].innerText = GetErrorText()