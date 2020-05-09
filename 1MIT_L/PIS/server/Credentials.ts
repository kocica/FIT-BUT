import {Association, DataTypes, InitOptions, ModelAttributes, HasOneGetAssociationMixin, HasOneSetAssociationMixin, HasOneCreateAssociationMixin} from 'sequelize'
import { Sequelizable } from './Sequelizable'
import { Session } from './Session'
import { BankAdmin } from './BankAdmin'
import { BankEmployee } from './BankEmployee'
import { sequelize } from './SequelizeInstance'

export interface CredentialsAttributes
{
	login: string
	password: string //should be hashed, salted and so on, but this is a school project¯\_(ツ)_/¯
	isTerminated: boolean
	type: "employee" | "admin" //employee, admin, ...?
}

export class Credentials extends Sequelizable implements CredentialsAttributes
{
	public login!: string
	public password!: string //should be hashed, salted and so on, but this is a school project¯\_(ツ)_/¯
	public isTerminated!: boolean
	public type!: "employee" | "admin" //employee, admin, ...?

	// Has one Session
	getSession!: HasOneGetAssociationMixin<Session>;
	setSession!: HasOneSetAssociationMixin<Session, number>;
	createSession!: HasOneCreateAssociationMixin<Session>;

	public readonly sessions!: Session[]

	public static associations: {
		sessions: Association<Credentials, Session>
	}

	public static getSequelizeInitArgs(): [ModelAttributes, InitOptions<Credentials>]
	{
		return [
			{
				login: {
					type: DataTypes.STRING(64),
					primaryKey: true
				},
				password: {
					type: DataTypes.STRING(64),
					allowNull: false,
				},
				isTerminated: {
					type: DataTypes.BOOLEAN,
					allowNull: false,
					defaultValue: false
				},
				type: {
					type: DataTypes.ENUM("admin", "employee"),
					allowNull: false
				}
			},
			{sequelize:sequelize}
		]
	}

	public static createAssociations(): void
	{
		this.hasOne(Session, {onDelete: 'CASCADE', foreignKey: {field: "CredentialsLogin"}})
	}

	public async terminate()
	{
		this.isTerminated = true
		let session = await this.getSession()
		if(session)
		{
			session.destroy()
		}
		return this.save()
	}

	public async getUser(): Promise<BankAdmin|BankEmployee>
	{
		let result = null
		if(this.type == "admin")
		{
			result = await BankAdmin.findOne({where: {CredentialsLogin: this.login}})
		}
		else if(this.type == "employee")
		{
			result = await BankEmployee.findOne({where: {CredentialsLogin: this.login}})
		}
		else
		{
			throw new Error("Unknown employee type")
		}
		if(result == null)
		{
			throw new Error("User with given credentials not found")
		}
		return result
	}

}
