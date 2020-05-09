import { Model, DataTypes, BelongsToGetAssociationMixin, BelongsToSetAssociationMixin, InitOptions, ModelAttributes, Association } from "sequelize";
import { Credentials } from "./Credentials";
import {Sequelizable} from "./Sequelizable"
import { sequelize } from "./SequelizeInstance";

export interface SessionContent
{
	[key: string]: any
	currentClient: number|undefined
	selectedAccount: number|undefined
}

export class Session extends Sequelizable
{
	public id!: number
	private content!: SessionContent
	public getCredentials!: BelongsToGetAssociationMixin<Credentials>
	public setCredentials!: BelongsToSetAssociationMixin<Credentials, string>

	public static associations: {
		Credentials: Association<Session, Credentials>
	}

	public static getSequelizeInitArgs(): [ModelAttributes, InitOptions<Session>]
	{
		return [
			{
				id: {
					type: DataTypes.INTEGER.UNSIGNED,
					autoIncrement: true, //should be randomly generated to be unguessable, but ¯\_(ツ)_/¯
					primaryKey: true
				},
				content: {
					type: DataTypes.JSON,
					allowNull: false
				}
			},
			{sequelize: sequelize}
		]
	}

	public static createAssociations(): void
	{
		Session.belongsTo(Credentials, {as: "Credentials"})
	}

	public getContent(): SessionContent
	{
		return this.content
	}

	public setContent(content: SessionContent)
	{
		this.content = content
	}

	public changeContent(changeCallback: (content: SessionContent) => void, save: boolean = true): Promise<Session>
	{
		let content = this.content
		changeCallback(content)
		this.content = content
		if(save)
		{
			return this.save()
		}
		return Promise.resolve(this)
	}

}