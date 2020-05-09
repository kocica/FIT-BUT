interface ApiBankEmployee
{
	login:     string
    isTerminated: boolean
}

type GetBankAccountListResponse22 = ApiBankEmployee[]

interface GetBankAccountListRequest2
{}

interface GetBankAccountListPrefixRequest
{
	prefixId: string
}
