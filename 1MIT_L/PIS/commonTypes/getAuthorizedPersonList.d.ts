interface ApiClient
{
	id:        number
	name:      string
	birthdate: string | Date
}

type GetAuthorizedClientListResponse = ApiClient[]

interface GetAuthorizedClientListRequest
{
	accountId: number
}