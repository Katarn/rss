import { Database } from "./database";
import { User } from "./user";

export class Sessions {
    private users: User[] = [];

    // =================================================================================================================

    constructor() {
        //
    }

    // =================================================================================================================

    public async init() {
        const db = await Database.getInstance();

        const results = await db.execute("SELECT * FROM sessions");

        for (const session of results) {
            this.users.push(new User(session));
        }
    }

    // =================================================================================================================

    public isSessionActive(_token: string): boolean {
        for (const user of this.users) {
            if (!user.isExpired()) {
                return true;
            }
        }

        return false;
    }

    // =================================================================================================================

    public getUser(token: string): User | null {
        for (const user of this.users)  {
            if (user.getToken() === token) {
                return user;
            }
        }

        return null;
    }
}
