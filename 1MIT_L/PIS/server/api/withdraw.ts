/// <reference path="../../commonTypes/withdraw.d.ts" />

import { Server } from "@hapi/hapi";
import { BankEmployee } from "../BankEmployee";
import { getCredentialsAndSession } from "./auth";
import { BankAccount } from "../BankAccount";

function isWithdrawRequestData(obj: any): obj is WithdrawRequestData
{
	return typeof obj == "object" // && typeof obj.accountId == "string" && typeof obj.amount == "string"
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/withdraw",
			handler: async (request, h) => {
                let payload = request.payload
                if(!isWithdrawRequestData(payload))
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

				// TODO: Set currentClient
				try
				{
					await employee.withdraw(account!.number, parseInt(payload.amount))
				}
				catch(e)
				{
					return h.response("").redirect("/static/html/employee/withdraw.html").state("withdrawalFailed", "true", {ttl: 10000, path: "/", isSecure: false, isHttpOnly: false})
				}

				// Redirect and show account
				return h.redirect(`/static/html/employee/showAccount.html?ban=${account!.number}`)
			}
		}
	])
}