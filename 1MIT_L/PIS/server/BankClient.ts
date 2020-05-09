import {HasManyGetAssociationsMixin, HasManyAddAssociationMixin, HasManyHasAssociationMixin, HasManyCountAssociationsMixin, HasManyCreateAssociationMixin, HasManySetAssociationsMixin, HasManyAddAssociationsMixin, HasManyRemoveAssociationMixin, HasManyRemoveAssociationsMixin, HasManyHasAssociationsMixin, BelongsToManyGetAssociationsMixin, BelongsToManySetAssociationsMixin, BelongsToManyAddAssociationsMixin, BelongsToManyAddAssociationMixin, BelongsToManyCreateAssociationMixin, BelongsToManyRemoveAssociationMixin, BelongsToManyRemoveAssociationsMixin, BelongsToManyHasAssociationMixin, BelongsToManyHasAssociationsMixin, BelongsToManyCountAssociationsMixin, ModelAttributes, InitOptions, DataTypes} from 'sequelize'
import { Sequelizable } from './Sequelizable'
import { BankAccount } from './BankAccount';
import { sequelize } from './SequelizeInstance';

/**
 * @brief Class representing bank client.
 */
export class BankClient extends Sequelizable
{
    public id!:    number;
    public name!:  string;
    public bday!:  Date;

    // Has many OwnedAccount
    getOwnedAccounts!: HasManyGetAssociationsMixin<BankAccount>;
    setOwnedAccounts!: HasManySetAssociationsMixin<BankAccount, number>;
    addOwnedAccounts!: HasManyAddAssociationsMixin<BankAccount, number>;
    addOwnedAccount!: HasManyAddAssociationMixin<BankAccount, number>;
    createOwnedAccount!: HasManyCreateAssociationMixin<BankAccount>;
    removeOwnedAccount!: HasManyRemoveAssociationMixin<BankAccount, number>;
    removeOwnedAccounts!: HasManyRemoveAssociationsMixin<BankAccount, number>;
    hasOwnedAccount!: HasManyHasAssociationMixin<BankAccount, number>;
    hasOwnedAccounts!: HasManyHasAssociationsMixin<BankAccount, number>;
    countOwnedAccounts!: HasManyCountAssociationsMixin;

    // Belongs to many AuthorizedAccount
    getAuthorizedAccounts!: BelongsToManyGetAssociationsMixin<BankAccount>;
    setAuthorizedAccount!: BelongsToManySetAssociationsMixin<BankAccount, number>;
    addAuthorizedAccounts!: BelongsToManyAddAssociationsMixin<BankAccount, number>;
    addAuthorizedAccount!: BelongsToManyAddAssociationMixin<BankAccount, number>;
    createAuthorizedAccount!: BelongsToManyCreateAssociationMixin<BankAccount>;
    removeAuthorizedAccount!: BelongsToManyRemoveAssociationMixin<BankAccount, number>;
    removeAuthorizedAccounts!: BelongsToManyRemoveAssociationsMixin<BankAccount, number>;
    hasAuthorizedAccount!: BelongsToManyHasAssociationMixin<BankAccount, number>;
    hasAuthorizedAccounts!: BelongsToManyHasAssociationsMixin<BankAccount, number>;
    countAuthorizedAccount!: BelongsToManyCountAssociationsMixin;

    public static getSequelizeInitArgs(): [ModelAttributes, InitOptions<BankClient>]
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
                bday: {
                    type: DataTypes.DATE,
                    allowNull: false,
                }
            },
            {sequelize:sequelize}
        ]
    }

    public static createAssociations()
    {
        BankClient.hasMany(BankAccount, {as: "ownedAccounts"})
        BankClient.belongsToMany(BankAccount, {as: "authorizedAccounts", through: "accountAuthorizations"})
    }
}