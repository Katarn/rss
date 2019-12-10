export class User {
    private token: string;
    private expiration: Date;

    // =================================================================================================================

    constructor(params: any) {
        this.token      = params.token;
        this.expiration = params.expiration;
    }

    // =================================================================================================================

    public isExpired() {
        const now = new Date();

        if (this.expiration > now) {
            return true;
        }

        return false;
    }

    // =================================================================================================================

    public getToken() {
        return this.token;
    }
}
