interface ApiBankAccount
{
	number:     number
    identifier: string
    amount:     number
    ownerId:    number
}

type GetBankAccountListResponse = ApiBankAccount[]

interface GetBankAccountListRequest
{
	clientId: number
}

interface GetBankAccountListPrefixRequest
{
	prefixId: string
}
