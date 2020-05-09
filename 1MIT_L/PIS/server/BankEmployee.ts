import {ModelAttributes, InitOptions, DataTypes, HasOneGetAssociationMixin, HasOneSetAssociationMixin, HasOneCreateAssociationMixin, Transaction} from 'sequelize'
import { Sequelizable } from './Sequelizable'
import { BankClient } from './BankClient';
import { BankAccount } from './BankAccount';
import { sequelize } from './SequelizeInstance';
import { Credentials } from './Credentials';

import { BankTransaction } from './BankTransaction';

/**
 * @brief Class used to manage employee accounts in the system.
 */
export class BankEmployee extends Sequelizable
{
    public id!: number;
    public name!:  string;
    public birthdate!:  Date;

    // Has one Credentials
    getCredentials!: HasOneGetAssociationMixin<Credentials>;
    setCredentials!: HasOneSetAssociationMixin<Credentials, string>;
    createCredentials!: HasOneCreateAssociationMixin<Credentials>;

    public static getSequelizeInitArgs(): [ModelAttributes, InitOptions<BankEmployee>]
    {
        return [
            {
                id: {
                    type: DataTypes.INTEGER.UNSIGNED,
                    autoIncrement: true,
                    primaryKey: true
                },
                name: {
                    type: DataTypes.STRING(64),
                    allowNull: false,
                },
                birthdate: {
                    type: DataTypes.DATE,
                    allowNull: false,
                },
            },
            {sequelize:sequelize}
        ]
    }

    public static createAssociations()
    {
        BankEmployee.belongsTo(Credentials, {as: "Credentials", foreignKey: {allowNull: false}})
    }

    /**
     * Store new client to the DB
     * @param name 
     * @param birthdate 
     */
    public addClient( name : string, birthdate : Date, transaction?: Transaction)
    {
        return BankClient.create( { name: name, bday : birthdate }, {transaction: transaction})
    }

    /**
     * Creates an account for a client
     * @param number 
     */
    public async createAccount(client: BankClient, transaction?: Transaction): Promise<BankAccount>
    {
        if(typeof transaction === "undefined")
        {
            return sequelize.transaction(t => this.createAccount(client, t))
        }
        let acc = await BankAccount.create({ownerId: client.id}, {transaction: transaction})
        return acc;
    }

    public addAuthorizedPerson(account: BankAccount, person: BankClient, transaction?: Transaction)
    {
        return account.addAuthorizedPerson(person, {transaction: transaction})
    }
    
    public removeAuthorizedPerson(account: BankAccount, person: BankClient, transaction?: Transaction)
    {
        return account.removeAuthorizedPerson(person, {transaction: transaction})
    }

    private getClientPromise(transaction: Transaction): Promise<BankClient>
    {
        let sessionP = this.getCredentials({transaction: transaction}).then(credentials => credentials.getSession({transaction: transaction}))
        return sessionP.then(async session => {
            let clientId = session.getContent().currentClient
            if(typeof clientId === "undefined")
            {
                return null
            }
            return BankClient.findByPk(clientId)
        })
    }

    private getAccountPromises(transaction: Transaction, account: number): [Promise<BankAccount>]
    private getAccountPromises(transaction: Transaction, account1: number, account2: number|string): [Promise<BankAccount>, Promise<BankAccount|null>]
    private getAccountPromises(transaction: Transaction, ...accIdentifiers: (number|string)[]): Promise<BankAccount|null>[]
    {
        return accIdentifiers.map(async accId => {
            let account: BankAccount|null
            if(typeof accId === "number")
            {
                account = await BankAccount.findByPk(accId, {transaction: transaction})
                if(!account)
                {
                    throw new Error(`Account with number ${accId} not found`)
                }
            }
            else
            {
                account = await BankAccount.findOne({where: {identifier: accId}, transaction: transaction})
            }
            return account
        })
    }

    // Record a deposit of money to an account
    public async deposit( accNumber : number, amount : number, transaction?: Transaction): Promise<BankTransaction>
    {
        if(amount <= 0)
        {
            throw new Error("Deposited amount must be positive")
        }
        //ensure that the operation is transacted
        if(typeof transaction === "undefined")
        {
            return sequelize.transaction(t => this.deposit(accNumber, amount, t))
        }
        let accountP = this.getAccountPromises(transaction, accNumber)[0]
        let clientP = this.getClientPromise(transaction)
        let account = await accountP
        this.checkClientAuthorization(await clientP, account)
        let alterP = account.alterBalance(amount, transaction)
        let transRecordP = BankTransaction.doCreate("deposit", amount, await clientP, this, transaction, account.identifier)
        return (await Promise.all([alterP, transRecordP]))[1];
    }

    private async checkClientAuthorization(client: BankClient, account: BankAccount): Promise<void>
    {
        if(!account.isClientAuthorized(client))
        {
            throw new Error(`Client id ${client.id} is not authorized to perform actions on account number ${account.number}`)
        }
    }

    //record a withdrawal of money from an account
    public async withdraw( accNumber : number, amount : number, transaction?: Transaction): Promise<BankTransaction>
    {
        if(amount <= 0)
        {
            throw new Error("Withdrawn amount must be positive")
        }
        //ensure that the operation is transacted
        if(typeof transaction === "undefined")
        {
            return sequelize.transaction(t => this.withdraw(accNumber, amount, t))
        }
        let client = await this.getClientPromise(transaction)
        if(!client)
        {
            throw new Error("No client selected")
        }
        let account = await this.getAccountPromises(transaction, accNumber)[0]
        await this.checkClientAuthorization(client, account)
        let alterPromise = account.alterBalance(-amount, transaction)
        let transRecordPromise = BankTransaction.doCreate("withdrawal", amount, client, this, transaction, account.identifier)
        return (await Promise.all([alterPromise, transRecordPromise]))[1];
    }

    /**
     * Transfers money from accoutn identified by sourceAccountNum to account identified by destinationAccountId
     * @param sourceAccountNum Number of the account from which money will be transfered
     * @param destinationAccountId Number of the account to which money will be transfered.
     * If number, it is the number of an account in this bank.
     * If string, it is the generic account identifier and can be from a different bank
     * @param amount The amount of money to transfer
     * @param transaction The transaction to do the operation under. If undefined, will be done under its own transaction
     */
    public async transfer(sourceAccountNum: number, destinationAccountId: number|string, amount: number, transaction?: Transaction): Promise<BankTransaction>
    {
        if(amount <= 0)
        {
            throw new Error("Withdrawn amount must be positive")
        }
        //ensure that the operation is transacted
        if(typeof transaction === "undefined")
        {
            return sequelize.transaction(t => this.transfer(sourceAccountNum, destinationAccountId, amount, t))
        }
        let client = await this.getClientPromise(transaction)
        if(!client)
        {
            throw new Error("No client selected")
        }
        let [srcAccountP, dstAccountP] = this.getAccountPromises(transaction, sourceAccountNum, destinationAccountId)

        let srcAccount = await srcAccountP
        await this.checkClientAuthorization(client, srcAccount)
        let alterPromiseSrc = srcAccount.alterBalance(-amount, transaction)
        let dstAccount = await dstAccountP
        let alterPromiseDst = dstAccount ? dstAccount.alterBalance(amount, transaction) : Promise.resolve()
        let transRecordPromise = BankTransaction.doCreate(
            "transfer",
            amount,
            client,
            this,
            transaction,
            srcAccount.identifier,
            dstAccount?.identifier ?? destinationAccountId as string
        )
        return (await Promise.all([alterPromiseSrc, alterPromiseDst, transRecordPromise]))[2];
    }
}