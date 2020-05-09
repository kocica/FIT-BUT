import {Association, HasManyGetAssociationsMixin, HasManyAddAssociationMixin, HasManyHasAssociationMixin, HasManyCountAssociationsMixin, HasManyCreateAssociationMixin, Model, ModelAttributes, InitOptions, DataTypes, HasOneGetAssociationMixin, HasOneSetAssociationMixin, HasOneCreateAssociationMixin} from 'sequelize'
import { Credentials } from "./Credentials";
import { BankEmployee } from "./BankEmployee";
import { Sequelizable } from './Sequelizable'
import { sequelize } from './SequelizeInstance';
import { Op } from "sequelize"

/**
 * @brief Class used to manage admin accounts in the system.
 */
export class BankAdmin extends Sequelizable
{

	public id!: number
	// Has one Credentials
	getCredentials!: HasOneGetAssociationMixin<Credentials>;
	setCredentials!: HasOneSetAssociationMixin<Credentials, string>;
	createCredentials!: HasOneCreateAssociationMixin<Credentials>;

	public static associations: {
		Credentials: Association<BankAdmin, Credentials>
	}

	public static getSequelizeInitArgs(): [ModelAttributes, InitOptions<BankAdmin>]
	{
		return [
			{
				id: {
					type: DataTypes.INTEGER.UNSIGNED,
					autoIncrement: true,
					primaryKey: true
				}
			},
			{sequelize:sequelize}
		]
	}

	public static createAssociations()
	{
		BankAdmin.belongsTo(Credentials, {as: "Credentials"})
	}

	async addEmployee( login : string, password : string, name : string, birthdate : Date )
	{
		return BankEmployee.create(
			{
				name: name,
				birthdate: birthdate,
				Credentials: {
					login: login,
					password: password,
					type: "employee"
				}
			},
			{
				include: [
					{association: BankAdmin.associations.Credentials}
				]
			}
		);
	}

	async removeEmployee( login : string )
	{
		let credentials = await Credentials.findOne({
			where: {
				login: {
					[Op.eq]: login
				}
			}
		})

		if(!credentials || credentials.type == "admin")
		{
			throw new Error("Employee with specified login doesnt exist or its admin login")
		}

		let employee = await BankEmployee.findOne({where: {CredentialsLogin: login}})

		credentials.terminate()
		employee?.destroy()
	}
}