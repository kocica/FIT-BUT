interface ApiClient
{
	id: number
	name: string
	birthdate: string|Date //date string
}

type GetClientListResponse = ApiClient[]

interface GetClientListRequest
{
	namePrefix?: string
}