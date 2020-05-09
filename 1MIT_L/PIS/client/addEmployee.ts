/// <reference path="../commonTypes/employee.d.ts" />

async function createNewEmployee()
{
    let login = $("#employeeIdInput").val()!.toString()
    let password1 = $("#employeePswInput").val()!.toString()
    let password2 = $("#employeePswInputRe").val()!.toString()
    let name = $("#employeeNameInput").val()!.toString()
    let bday = $("#employeeBdayInput").val()!.toString()
    let err = 0;
    
    if (login.length < 4)
    {
    	document.getElementById('errorMsglogin')!.innerHTML="Login is too short";
        err = 1;
    }else
    {
        document.getElementById('errorMsglogin')!.innerHTML="";
    }
    
    if (name.length < 4)
    {
    	document.getElementById('errorMsgname')!.innerHTML="Name is too short";
        err = 1;
    }else
    {
        document.getElementById('errorMsgname')!.innerHTML="";
    }

    if( password1.length < 5 )
    {
        document.getElementById('errorMsg1')!.innerHTML="Passsword is too short";
        err = 1;
    }else
    {
        document.getElementById('errorMsg1')!.innerHTML="";
    }

    if( password1 != password2 )
    {
        document.getElementById('errorMsg')!.innerHTML="Passswords does not match";
        err = 1;
    }else
    {
        document.getElementById('errorMsg')!.innerHTML="";
    }
    
    if( bday.length < 2 )
    {
        document.getElementById('errorMsgdate')!.innerHTML="Bad Day";
        err = 1;
    }else
    {
        document.getElementById('errorMsgdate')!.innerHTML="";
    }
    
    if(err == 1)
    {
        return; // Abort addition, cannot proceed
    }

    let req: AddEmployeeRequest = {
        login     : login,
        password  : password1,
        name      : name,
        birthdate : new Date(Date.parse(bday))
    }

    let _ = await new Promise(resolve => $.post("/api/addEmployee", req, resolve));

    window.location.reload(false);
}
