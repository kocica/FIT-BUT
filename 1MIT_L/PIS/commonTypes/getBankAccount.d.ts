interface ApiBankAccount
{
	number:     number
    identifier: string
    amount:     number
    ownerId:    number
}

type GetBankAccountResponse = ApiBankAccount

interface GetBankAccountRequest
{
    number: number
}