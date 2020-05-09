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

type GetBankTransactionListResponse = ApiBankTransaction[]

interface GetBankTransactionListByTypeRequest
{
    type: string
}

interface GetBankTransactionListBySrcRequest
{
    srcAcc: string
}

interface GetBankTransactionListByDestRequest
{
    destAcc: string
}