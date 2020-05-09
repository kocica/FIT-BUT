/// <reference path="../commonTypes/createClient.d.ts" />

async function onCreateClientSubmit()
{
    let login = $("#clientNameInput").val()!.toString()
    let bday = $("#clientBdayInput").val()!.toString()

    if( login.length < 5 )
    {
        document.getElementById('errorMsg')!.innerHTML="Login has to be of length at least 5 characters.";
        return; // Abort creation, cannot proceed
    }
    
    let req: CreateClientRequestData = {
        clientName : login,
        clientBday : new Date(Date.parse(bday))
    }

    let _ = await new Promise(resolve => $.post("/api/createClient", req, resolve));

    window.location.reload(false);

    // TODO: Assign bank accounts, etc.?
	//$("#submitHelperForm").submit()
}