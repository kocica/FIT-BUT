import { Session } from "./Session"
import { DataTypes, INTEGER, DATE } from "sequelize"
import { sequelize } from "./SequelizeInstance"
import { Credentials } from "./Credentials"
import { BankAccount } from "./BankAccount"
import { BankClient } from "./BankClient"
import { BankTransaction } from "./BankTransaction"
import { BankEmployee } from "./BankEmployee"
import { BankAdmin } from "./BankAdmin"

//first INIT ALL THE MODELS. It si necessary beause doing anything with an uninited model leads to errors

Credentials.init(
	...Credentials.getSequelizeInitArgs()
)

Session.init(
	...Session.getSequelizeInitArgs()
)

BankAccount.init(
	...BankAccount.getSequelizeInitArgs()
)

BankClient.init(
	...BankClient.getSequelizeInitArgs()
)

BankTransaction.init(
	...BankTransaction.getSequelizeInitArgs()
)

BankEmployee.init(
	...BankEmployee.getSequelizeInitArgs()
)

BankAdmin.init(
	...BankAdmin.getSequelizeInitArgs()
)


//----inits end here, put associations below here

Credentials.createAssociations()

Session.createAssociations()

BankAccount.createAssociations()

BankClient.createAssociations()

BankTransaction.createAssociations()

BankEmployee.createAssociations()

BankAdmin.createAssociations();