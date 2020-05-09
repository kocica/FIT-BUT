/// <reference path="../../commonTypes/employee.d.ts" />
/// <reference path="../../commonTypes/getEmployeeList.d.ts" />

import { Server } from "@hapi/hapi";
import { BankAdmin } from "../BankAdmin";
import { getCredentialsAndSession } from "./auth";
import { Credentials } from "../Credentials";

export function isRemoveEmployeeRequest(obj: any): obj is RemoveEmployeeRequest
{
    return typeof obj == "object" && typeof obj.login == "string"
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/removeEmployee",
			handler: async (request, h) => {
				let payload = request.payload

				if(!isRemoveEmployeeRequest(payload))
				{
					return h.response().code(400)
				}

				let [credentials, _] = await getCredentialsAndSession(request)

				if(credentials.type != "admin")
				{
					return h.response().code(403)
				}

				let admin = await credentials.getUser() as BankAdmin

				try
				{
					await admin.removeEmployee(payload.login)
				}
				catch(e)
				{
					return h.redirect("/static/html/admin/removeEmployee.html").state("employeeDoesntExist", "true", {ttl: 10000, path: "/", isSecure: false, isHttpOnly: false})
				}

				return h.redirect("/static/html/admin/removeEmployee.html").state("employeeRemoved", "true", {ttl: 10000, path: "/", isSecure: false, isHttpOnly: false})
			}
		},
    {
			method: "GET",
			path: "/api/getEmployeeList",
			handler: async (request, h) => {
				let payload = request.payload

				let accounts = await Credentials.findAll({where: {
					type: "employee",
          isTerminated: 0
				}})
				let response: GetBankAccountListResponse22 = accounts.map(account => {
					return {
						login : account.login,
						isTerminated: account.isTerminated,
					}
				})

				return h.response(response)
			}
		}
	])
}
