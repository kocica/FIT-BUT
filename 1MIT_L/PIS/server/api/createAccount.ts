import { Server } from "@hapi/hapi";
import { BankClient } from "../BankClient";
import { BankAccount } from "../BankAccount";
import { getCredentialsAndSession } from "./auth";
import { Credentials } from "../Credentials";
import { MakeErrorResponse } from "./commonFuncs";

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/createAccount",
			handler: async (request, h) => {

				let DoError = (err: string) => MakeErrorResponse(h, "/static/html/employee/createAccount.html", err)

				// Get logged-in employee
				let [credentials, session] = await getCredentialsAndSession(request)
				if(credentials.type != "employee")
				{
					return h.response().code(403)
				}

				let ownerId = session.getContent().currentClient
				if(!ownerId)
				{
					return DoError("No client seems to be selected")
				}

				// Get owner of new account
				let client = await BankClient.findOne({where: {
					id: ownerId
				}});

				if(!client)
				{
					return DoError("Failed to load the currently selected client")
				}

				// Create new account
				let account = await BankAccount.create({ownerId: client.id})

				// Add client as authorized person of his own account
				account.addAuthorizedPerson(client)

				// Redirect and show new account
				return h.redirect(`/static/html/employee/showAccount.html?ban=${account.number}`)
			}
		}
	])
}