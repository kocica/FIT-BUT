/// <reference path="../../commonTypes/deposit.d.ts" />

import { Server } from "@hapi/hapi";
import { BankEmployee } from "../BankEmployee";
import { getCredentialsAndSession } from "./auth";
import { Credentials } from "../Credentials";
import { BankAccount } from "../BankAccount";

function isDepositRequestData(obj: any): obj is DepositRequestData
{
	return typeof obj == "object" // && typeof obj.accountId == "string" && typeof obj.amount == "string"
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/deposit",
			handler: async (request, h) => {
                let payload = request.payload
                if(!isDepositRequestData(payload))
                {
                    return h.response().code(400)
                }

                // Get logged-in employee
                let [credentials, _] = await getCredentialsAndSession(request)
				if(credentials.type != "employee")
				{
					return h.response().code(403)
                }

                let employee = await credentials.getUser() as BankEmployee

                let account = await BankAccount.findOne({ where: { identifier: payload.accountId }})

				try
				{
					await employee.deposit(account!.number, parseInt(payload.amount))
				}
				catch(e)
				{
					return h.response("").redirect("/static/html/employee/deposit.html").state("depositFailed", "true", {ttl: 10000, path: "/", isSecure: false, isHttpOnly: false})
				}

                // Redirect and show account
				return h.redirect(`/static/html/employee/showAccount.html?ban=${account!.number}`)
			}
		}
	])
}