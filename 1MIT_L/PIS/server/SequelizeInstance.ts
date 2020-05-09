import {Sequelize} from 'sequelize'

export const sequelize = new Sequelize('mysql://pis:pispass@localhost:3306/pisproj')