import {Association, HasManyGetAssociationsMixin, HasManyAddAssociationMixin, HasManyHasAssociationMixin, HasManyCountAssociationsMixin, HasManyCreateAssociationMixin, ModelAttributes, InitOptions, DataTypes, Transaction, BelongsToGetAssociationMixin, BelongsToSetAssociationMixin, BelongsToCreateAssociationMixin} from 'sequelize'
import { Sequelizable } from './Sequelizable'
import { sequelize } from './SequelizeInstance';
import { BankAccount } from './BankAccount';
import { BankClient } from './BankClient';
import { BankEmployee } from './BankEmployee';

export type TransactionType = "deposit"|"withdrawal"|"transfer"

/**
 * @brief Class represention bank transaction between two (or more) accounts
 */
export class BankTransaction extends Sequelizable
{
	public id!:      number;
	public amount!:  number;
	public type!:    TransactionType
	public date!:    Date;
	public sourceAccount!: string|null
	public destinationAccount!: string|null

	// Belongs to Requestor
	getRequestor!: BelongsToGetAssociationMixin<BankClient>;
	setRequestor!: BelongsToSetAssociationMixin<BankClient, number>;

	// Belongs to Executor
	getExecutor!: BelongsToGetAssociationMixin<BankEmployee>;
	setExecutor!: BelongsToSetAssociationMixin<BankEmployee, number>;

	public static getSequelizeInitArgs(): [ModelAttributes, InitOptions<BankTransaction>]
	{
		return [
			{
				id: {
					type: DataTypes.INTEGER.UNSIGNED,
					autoIncrement: true,
					primaryKey: true
				},
				type: {
					type: DataTypes.ENUM("deposit", "withdrawal", "transfer"),
					allowNull: false,
				},
				date: {
					type: DataTypes.DATE,
					allowNull: false,
				},
				amount: {
					type: DataTypes.INTEGER.UNSIGNED,
					allowNull: false,
				},
				sourceAccount: {
					type: DataTypes.STRING(50),
					allowNull: true
				},
				destinationAccount: {
					type: DataTypes.STRING(50),
					allowNull: true
				}
			},
			{
				sequelize: sequelize,
				validate: {
					hasNecessaryFields(this: BankTransaction) {
						if(!this.sourceAccount && this.type != "deposit")
						{
							throw new Error("non-deposit transactions must have an origin account")
						}
						if(!this.destinationAccount && this.type != "withdrawal")
						{
							throw new Error("non-withdrawal transactions must have a destination account")
						}
					},
					transactionIdentityCheck(this: BankTransaction, errCallback: (err?: any) => void) {
						let checkAccountExistence = async (accId?: string|null): Promise<string|undefined> => {
							if(!accId)
							{
								//no id, no error
								return
							}
							if(!accId.startsWith(BankAccount.thisBankPrefix))
							{
								//is from a different bank, assume existence
								return
							}
							//is from this bank, check existence
							if(null === await BankAccount.findOne({where: {identifier: accId}}))
							{
								return "Should be from this bank, but is not"
							}
						}
						checkAccountExistence(this.sourceAccount).then(result => {
							return result || checkAccountExistence(this.destinationAccount)
						}).then(errCallback)
					},
					isWithinBank(this: BankTransaction) {
						/// Returns 1 for ids of accounts from this bank, 0 for other (or nonexistent) ids
						function countOurAccount(id?: string|null): number {
							return id?.startsWith(BankAccount.thisBankPrefix) ? 1 : 0
						}
						if(countOurAccount(this.sourceAccount)+countOurAccount(this.destinationAccount) < 1)
						{
							throw new Error("Transaction must have at least one account that is from this bank")
						}
					}
				}
			}
		]
	}

	public static createAssociations()
	{
		this.belongsTo(BankClient, {as: "requestor", foreignKey: "requestorId"})
		this.belongsTo(BankEmployee, {as: "executor", foreignKey: "executorId"})
	}

	//account1 is BankAccount to ensure it is from out bank. account2 is string to support accounts from other banks
	//transaction is mandatory seeing as it doesn't make sense to create BankTransaction without modifying acount balance
	//requestor can be null on deposit because there's no need to limit deposits
	public static doCreate(type: "deposit", amount: number, requestor: BankClient|null, executor: BankEmployee, trans: Transaction, destinationAccount: string): Promise<BankTransaction>
	public static doCreate(type: "withdrawal", amount: number, requestor: BankClient, executor: BankEmployee, trans: Transaction, sourceAccount: string): Promise<BankTransaction>
	public static doCreate(type: "transfer", amount: number, requestor: BankClient, executor: BankEmployee, trans: Transaction, sourceAccount: string, destinationAccount: string): Promise<BankTransaction>
	public static doCreate(type: TransactionType, amount: number, requestor: BankClient|null, executor: BankEmployee, trans: Transaction, account1: string, account2?: string): Promise<BankTransaction>
	{
		let sourceAccount = null
		let destinationAccount = null
		switch (type) {
			case "deposit":
				destinationAccount = account1
				break;
			case "withdrawal":
				sourceAccount = account1
				break;
			case "transfer":
				sourceAccount = account1
				destinationAccount = <string>account2
				break;
			default:
				break;
		}
		return this.create(
			{
				amount: amount,
				date: Date.now(),
				sourceAccount: sourceAccount,
				destinationAccount: destinationAccount,
				type: type,
				requestorId: requestor?.id,
				executorId: executor.id
			},
			{transaction: trans}
		)
	}

}