interface ApiEmployee
{
    login:      string
    password:   string
    name:       string
    birthdate:  Date
}

type AddEmployeeRequest = ApiEmployee

interface RemoveEmployeeRequest
{
    login: string
}

// TODO:
interface SearchEmployeeListRequest
{
    loginPrefix: string
}

type SearchEmployeeListResponse = ApiEmployee[]
