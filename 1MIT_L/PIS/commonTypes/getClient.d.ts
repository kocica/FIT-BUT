interface ApiClient
{
	id: number
	name: string
	birthdate: string|Date //date string
}

type GetClientResponse = ApiClient

interface GetClientRequest
{
	id: number
}