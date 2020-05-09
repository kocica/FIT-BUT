import * as Hapi from '@hapi/hapi'

//it is necessary to import this as this inits the ORM model. Without it, the ORM will not work
import './modelsInit'
import { sequelize } from './SequelizeInstance'
import { BankAdmin } from './BankAdmin'
import { Credentials } from './Credentials'
import { BankTransaction } from './BankTransaction'
import { BankAccount } from './BankAccount'
import * as Auth from './api/auth'
import * as AddAuthorizedPerson from './api/addAuthorizedPerson'
import * as RemoveAuthorizedPerson from './api/removeAuthorizedPerson'
import * as GetAuthorizedPersonList from './api/getAuthorizedPersonList'
import * as GetBankTransactionList from './api/getBankTransactionList'
import * as GetBankTransaction from './api/getBankTransaction'
import * as GetBankAccountList from './api/getBankAccountList'
import * as GetBankAccount from './api/getBankAccount'
import * as CreateAccount from './api/createAccount'
import * as GetClientList from './api/getClientList'
import * as GetClient from './api/getClient'
import * as CreateClient from './api/createClient'
import * as SelectClient from './api/selectClient'
import * as AddEmployee from './api/addEmployee'
import * as RemoveEmployee from './api/removeEmployee'
import * as Deposit from './api/deposit'
import * as Withdraw from './api/withdraw'
import * as Transfer from './api/transfer'
import * as SelectAccount from './api/selectAccount'
import * as Path from 'path'

async function init()
{
	let server = new Hapi.Server({
		port:4444,
		host: "localhost",
		routes: {
			files: {
				relativeTo: Path.join(__dirname, "..", "..", "static")
			}
		},
		debug: {
			request: ['error']
		}
	})
	await sequelize.sync()
	{
		//ensure existence of admin
		let adminFindOrCreateRes = (await BankAdmin.findOrCreate({
			where: {
				id: 1
			},
			defaults: {
				id: 1,
			}
		}))
		let admin = adminFindOrCreateRes[0]
		let credentials: Credentials
		if(adminFindOrCreateRes[1])
		{
			console.log(admin)
			credentials = await admin.createCredentials({
				login: "admin",
				password: "adminPass",
				type: "admin"
			})
			let employee = await admin.addEmployee("emp1", "emp1Pass", "Pepa", new Date)
			let client = await employee.addClient("Josef", new Date())
		}
		else
		{
			credentials = await admin.getCredentials()
		}
		//basic route. Routes will probably go into their own modules eventually
		await Auth.setupAuthentication(server)

		// Transaction routes
		GetBankTransactionList.AddEndpoints(server)
		GetBankTransaction.AddEndpoints(server)

		// Client routes
		GetClientList.AddEndpoints(server)
		GetClient.AddEndpoints(server)
		CreateClient.AddEndpoints(server)
		SelectClient.AddEndpoints(server)

		// Account routes
		GetBankAccountList.AddEndpoints(server)
		GetBankAccount.AddEndpoints(server)
		CreateAccount.AddEndpoints(server)

		// Authorization routes
		AddAuthorizedPerson.AddEndpoints(server)
		GetAuthorizedPersonList.AddEndpoints(server)
		RemoveAuthorizedPerson.AddEndpoints(server)

		// Employee routes
		AddEmployee.AddEndpoints(server)
		RemoveEmployee.AddEndpoints(server)

		// Account actions
		Deposit.AddEndpoints(server)
		Withdraw.AddEndpoints(server)
		Transfer.AddEndpoints(server)
		SelectAccount.AddEndpoints(server)

		await server.register(require("@hapi/inert"))
		server.route([
			{
				method: 'GET',
				path: "/static/public/{path*}",
				handler: {
					directory: {
						path: "./public/",
						index: false
					}
				},
				options: {
					auth: false
				}
			},
			{
				method: 'GET',
				path: "/static/{path*}",
				handler: {
					directory: {
						path: ".",
						index: false
					}
				}
			},
			{
				method: "GET",
				path: "/",
				handler: async (req, h) => {
					let [credentials, session] = await Auth.getCredentialsAndSession(req)
					return Auth.redirectToDefaultPage(credentials, h, session)
				}
			}
		])
		server.ext("onPreResponse", (request, h) => {
			return h.continue
		})
		server.route({
			method: 'GET',
			path: "/status",
			handler: async (request, h) => {
				let session = (await credentials.getSession()) ?? await credentials.createSession({content: {accessCount: 0}})
				session.changeContent(content => {content.accessCount += 1})
				let test = JSON.stringify(await BankAccount.findAll(), undefined, 2)
				return `hello. login: ${credentials.login}, pass: ${credentials.password}.</br>`+
				`Your session number is ${session.id}. Session was accessed ${session.getContent().accessCount} times.</br>`+
				`The list of transactions in the system: ${JSON.stringify(await BankTransaction.findAll(), undefined, 2).replace(/\n/g, "</br>")}</br>`+
				`The list of accounts: ${JSON.stringify(await BankAccount.findAll(), undefined, 2).replace(/\n/g, "</br>")}`
			}
		})
	}
	await server.start()
}

init().then(() => {
	console.log("server initialized successfully")
}, err => {
	console.log(`Server init failed: ${err}`)
	if(err.stack)
	{
		console.log(`Caught at:\n${err.stack}`)
	}
})