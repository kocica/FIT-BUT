/// <reference path="../../commonTypes/getBankAccount.d.ts" />


import { Server } from "@hapi/hapi";
import { BankAccount } from "../BankAccount";
import { Op } from "sequelize"

export function isGetBankAccountRequest(obj: any): obj is GetBankAccountRequest
{
	return typeof obj == "object" // && (typeof obj.number == "number" || typeof obj.number == "undefined")
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/getBankAccount",
			handler: async (request, h) => {
				let payload = request.payload
				if(!isGetBankAccountRequest(payload))
				{
					return h.response().code(400)
				}
				let account = await BankAccount.findOne({where: {
					number: {
						[Op.eq]: payload.number
					}
				}})
                let response: GetBankAccountResponse = { number: account!.number,
                                                         identifier: account!.identifier,
                                                         amount: account!.amount,
                                                         ownerId: account!.ownerId }

				return h.response(response)
			}
		}
	])
}