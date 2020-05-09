/// <reference path="../../commonTypes/transfer.d.ts" />

import { Server } from "@hapi/hapi";
import { BankEmployee } from "../BankEmployee";
import { getCredentialsAndSession } from "./auth";
import { BankAccount } from "../BankAccount";
import { MakeErrorResponse } from "./commonFuncs";

function isTransferRequestData(obj: any): obj is TransferRequestData
{
	return typeof obj == "object" // && typeof obj.accountIdSrc == "string" && && typeof obj.accountIdDst == "string" && typeof obj.amount == "string"
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/transfer",
			handler: async (request, h) => {
				let DoError = (err: string) => MakeErrorResponse(h, "/static/html/employee/transfer.html", err)
				let payload = request.payload
				if(!isTransferRequestData(payload))
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

				let accountSrc = await BankAccount.findOne({ where: { identifier: payload.accountIdSrc }})
				if(!accountSrc)
				{
					return DoError("Source account not found")
				}
				let accountDst = await BankAccount.findOne({ where: { identifier: payload.accountIdDst }})

				// TODO: Display transaction detail instead of an account?
				try
				{
					await employee.transfer(accountSrc!.number, accountDst?.number ?? payload.accountIdDst, parseInt(payload.amount))
				}
				catch(e)
				{
					console.log(e)
					if(e instanceof Error)
					{
						e = e.message
					}
					return DoError(e)
				}

				// Redirect and show account
				return h.redirect(`/static/html/employee/showAccount.html?ban=${accountSrc.number}`)
			}
		}
	])
}