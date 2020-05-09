import { Model } from "sequelize";

export class Sequelizable extends Model
{
	// timestamps! (apparently required by sequelize)
	public readonly createdAt!: Date;
	public readonly updatedAt!: Date;
}