import * as mysql from "mysql2/promise";
import { config as Config } from "./config";

export class Database {
    private static instance: Database | null;
    private connection: mysql.Pool | undefined = undefined;

    // =================================================================================================================

    public static async getInstance(): Promise<Database> {
        if (!this.instance) {
            this.instance = new Database();

            await this.instance.connect();
        }

        return this.instance;
    }

    // =================================================================================================================

    public async getConnection() {
        return this.connection!.getConnection();
    }

    // =================================================================================================================

    public async connect() {
        this.connection = await mysql.createPool({
            host:     Config.dbHost,
            database: Config.dbName,
            user:     Config.dbUser,
            password: Config.dbPassword,

            waitForConnections: true,
            connectionLimit:    50,
            queueLimit:         0,

            charset:     "utf8mb4_general_ci",
            dateStrings: true
        });
    }

    // =================================================================================================================

    public disconnect() {
        this.connection!.end();
    }

    // =================================================================================================================

    public async execute(query: string, params: any = null): Promise<mysql.RowDataPacket[]> {
        const [ results ] = await this.connection!.execute(query, params);

        return results as mysql.RowDataPacket[];
    }

    // =================================================================================================================

    public async query(query: string, params: any = null) {
        return await this.connection!.query(query, params);
    }
}
