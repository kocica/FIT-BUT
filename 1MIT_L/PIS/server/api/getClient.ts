/// <reference path="../../commonTypes/getClient.d.ts" />


import { Server } from "@hapi/hapi";
import { BankClient } from "../BankClient";
import { Op } from "sequelize"
import { getCredentialsAndSession } from "./auth";

export function isGetClientRequest(obj: any): obj is GetClientRequest
{
	return typeof obj == "object"
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/getClientFromSession",
			handler: async (request, h) => {
				let [_, session] = await getCredentialsAndSession(request)
				let client = await BankClient.findOne({where: {
					id: {
						[Op.eq]: session.getContent().currentClient
					}
				}})
				let response: GetClientResponse = { name: client!.name, id: client!.id, birthdate: client!.bday.toDateString() }
				return h.response(response)
			}
		},
		{
			method: "POST",
			path: "/api/getClientById",
			handler: async (request, h) => {
				let payload = request.payload
				if(!isGetClientRequest(payload))
				{
					return h.response().code(400)
				}
				let client = await BankClient.findOne({where: {
					id: {
						[Op.eq]: payload.id
					}
				}})
				let response: GetClientResponse = { name: client!.name, id: client!.id, birthdate: client!.bday }

				return h.response(response)
			}
		}
	])
}