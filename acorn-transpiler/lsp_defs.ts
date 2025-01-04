interface Message {
    jsonrpc: string; // inline comment
}

interface RequestMessage extends Message {

    /**
     * The request id.
     */
    id: integer | string;

    /**
     * The method to be invoked.
     */
    method: string;

    /**
     * The method's params.
     */
    params?: array | object;
}

interface ResponseMessage extends Message {
    /**
     * The request id.
     */
    id: integer | string | null;

    /**
     * The result of a request. This member is REQUIRED on success.
     * This member MUST NOT exist if there was an error invoking the method.
     */
    result?: LSPAny;

    /**
     * The error object in case a request fails.
     */
    error?: ResponseError;
}
