import {BelongsToGetAssociationMixin, BelongsToSetAssociationMixin, BelongsToCreateAssociationMixin, Transaction, ModelAttributes, InitOptions, DataTypes, BelongsToManyGetAssociationsMixin, BelongsToManySetAssociationsMixin, BelongsToManyAddAssociationsMixin, BelongsToManyAddAssociationMixin, BelongsToManyCreateAssociationMixin, BelongsToManyRemoveAssociationMixin, BelongsToManyRemoveAssociationsMixin, BelongsToManyHasAssociationMixin, BelongsToManyHasAssociationsMixin, BelongsToManyCountAssociationsMixin} from 'sequelize'
import { Sequelizable } from './Sequelizable'
import { BankClient } from './BankClient';
import { sequelize } from './SequelizeInstance';

/**
 * @brief Class used to manage bank account.
 */
export class BankAccount extends Sequelizable
{
    public static readonly thisBankPrefix = "PISBANKACCOUNT"
    public number!:   number;
    ///Global account identifier (like IBAN)
    public identifier!: string
    public amount!:   number;
    public ownerId!: number
    // Belongs to BankClient
    getOwner!: BelongsToGetAssociationMixin<BankClient>;
    setOwner!: BelongsToSetAssociationMixin<BankClient, number>;
    createOwner!: BelongsToCreateAssociationMixin<BankClient>;

    // Belongs to many AuthorizedPerson
    getAuthorizedPersons!: BelongsToManyGetAssociationsMixin<BankClient>;
    setAuthorizedPerson!: BelongsToManySetAssociationsMixin<BankClient, number>;
    addAuthorizedPersons!: BelongsToManyAddAssociationsMixin<BankClient, number>;
    addAuthorizedPerson!: BelongsToManyAddAssociationMixin<BankClient, number>;
    createAuthorizedPerson!: BelongsToManyCreateAssociationMixin<BankClient>;
    removeAuthorizedPerson!: BelongsToManyRemoveAssociationMixin<BankClient, number>;
    removeAuthorizedPersons!: BelongsToManyRemoveAssociationsMixin<BankClient, number>;
    hasAuthorizedPerson!: BelongsToManyHasAssociationMixin<BankClient, number>;
    hasAuthorizedPersons!: BelongsToManyHasAssociationsMixin<BankClient, number>;
    countAuthorizedPerson!: BelongsToManyCountAssociationsMixin;

    public static getSequelizeInitArgs(): [ModelAttributes, InitOptions<BankAccount>]
    {
        return [
            {
                number: {
                    type: DataTypes.INTEGER.UNSIGNED,
                    primaryKey: true
                },
                identifier: {
                    type: DataTypes.STRING(50),
                    unique: true
                },
                amount: {
                    type: DataTypes.INTEGER.UNSIGNED,
                    allowNull: false,
                    defaultValue: 0
                }
            },
            {
                sequelize: sequelize,
                hooks: {
                    beforeCreate: async (account) => {
                        if(account.number === null)
                        {
                            let maxNumAccount = await BankAccount.findOne({order: [["number", "DESC"]]})
                            if(!maxNumAccount)
                            {
                                //first account
                                account.number = 1
                            }
                            else
                            {
                                account.number = maxNumAccount.number+1
                            }
                        }
                        account.identifier = `${this.thisBankPrefix}${'0'.repeat(50-account.number.toString().length-this.thisBankPrefix.length)}${account.number}`
                    }
                }
            }
        ]
    }

    public static createAssociations()
    {
        BankAccount.belongsTo(BankClient, {as: "owner", foreignKey: {allowNull: false}})
        BankAccount.belongsToMany(BankClient, {as: "authorizedPersons", through: "accountAuthorizations"})
    }

    public async isClientAuthorized(client: number|BankClient): Promise<boolean>
    {
        if(typeof client != "number")
        {
            client = client.id
        }
        if(client == this.ownerId)
        {
            return true
        }
        //client is not owner, check if he's authorized
        return this.hasAuthorizedPerson(client)
    }

    /**
     * Alters the account balance by amount
     * @param amount The amount to alter the balance by. Negative deducts money
     * @param transaction The transaction to do this under. Can be undefined
     */
    public async alterBalance(amount: number, transaction?: Transaction): Promise<void>
    {
        if(this.amount+amount < 0)
        {
            throw new Error(`Tried to withdraw ${amount} money from account ${this.number}, which only has ${this.amount}`)
        }
        this.amount += amount;
        await this.save({transaction: transaction})
    }
}