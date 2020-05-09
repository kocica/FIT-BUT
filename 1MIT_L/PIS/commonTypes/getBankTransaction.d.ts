export type TransactionType = "deposit"|"withdrawal"|"transfer"

interface ApiBankTransaction
{
	id:                 number
	amount:             number
	type:               TransactionType
	date:               Date
	sourceAccount:      string|null
	destinationAccount: string|null
}

type GetBankTransactionResponse = ApiBankTransaction

interface GetBankTransactionRequest
{
    id: number
}