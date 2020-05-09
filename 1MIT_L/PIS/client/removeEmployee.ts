/// <reference path="../commonTypes/employee.d.ts" />

async function removeEmployee()
{
    let login = $("#employeeIdInput").val()!.toString()

    if( login.length < 4 ) // Login must have at least 4 characters
    {
        document.getElementById('errorMsg')!.innerHTML="Login is too short.";
        return; // Abort removal, cannot proceed
    }

    let req: RemoveEmployeeRequest = {
        login     : login
    }

    let _ = await new Promise(resolve => $.post("/api/removeEmployee", req, resolve));

    window.location.reload(false);
}