/// <reference path="../../commonTypes/getBankTransaction.d.ts" />

import { Server } from "@hapi/hapi";
import { BankTransaction } from "../BankTransaction";
import { Op } from "sequelize"
import { GetBankTransactionRequest, GetBankTransactionResponse } from "../../commonTypes/getBankTransaction";

export function isGetBankTransactionRequest(obj: any): obj is GetBankTransactionRequest
{
	return typeof obj == "object" // && (typeof obj.id == "number" || typeof obj.id == "undefined")
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/getBankTransaction",
			handler: async (request, h) => {
				let payload = request.payload
				if(!isGetBankTransactionRequest(payload))
				{
					return h.response().code(400)
				}
				let transaction = await BankTransaction.findOne({where: {
					id: {
						[Op.eq]: payload.id
					}
				}})
				let response: GetBankTransactionResponse =
					{
						id:                 transaction!.id,
						amount:             transaction!.amount,
						type:               transaction!.type,
						date:               transaction!.date,
						sourceAccount:      transaction!.sourceAccount,
						destinationAccount: transaction!.destinationAccount
					}
				return h.response(response)
			}
		}
	])
}