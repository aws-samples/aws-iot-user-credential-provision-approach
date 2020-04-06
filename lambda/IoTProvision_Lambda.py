import boto3
import json
import logging

logger = logging.getLogger()
logger.setLevel(logging.INFO)

iot = boto3.client('iot')
dynamodb = boto3.client('dynamodb')
table = 'DSN'

# Create a policy
policyDocument = {
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": "iot:*",
      "Resource": "*"
    }
  ]
}

def delete_cert_and_policy(deviceId, principals):
    for principal in principals:
        certificateId = principal.split('/')[-1]
        policies = iot.list_attached_policies(target=principal)
        for policy in policies['policies']:
            iot.detach_policy(
                policyName=policy['policyName'], target=principal
            )
            iot.delete_policy(policyName=policy['policyName'])
        iot.update_certificate(
            certificateId=certificateId, newStatus='INACTIVE'
        )
#        iot.detach_thing_principal(
#            thingName=deviceId,
#            principal=principal
#        )
#        wait_for_detach_finish(deviceID, principal)
#        iot.delete_certificate(
#            certificateId=certificateId, forceDelete=True
#        )

def lambda_handler(event, context):
    logger.info("request: " + json.dumps(event));
    # check device serial number is valid
    deviceId = event['params']['querystring']['SN']
    deviceItem = dynamodb.get_item(TableName=table, Key={'sn': {'S': deviceId}})
    if not deviceItem.get('Item'):
        return {
            'statusCode': 403,
            'body': {'msg': 'Invalid device serial number'}
        }

    # create thing
    try:
        iot.describe_thing(
            thingName=deviceId
        )
    except iot.exceptions.ResourceNotFoundException:
        response = iot.create_thing(
            thingName=deviceId
        )

    # delete certificates which are attached to this thing
    response = iot.list_thing_principals(
        thingName=deviceId
    )
    if response['principals']:
        delete_cert_and_policy(deviceId, response['principals'])

    # generate keys and certificate
    response = iot.create_keys_and_certificate(setAsActive=True)
    certificateArn = response['certificateArn']
    certificatePem = response['certificatePem']
    privateKey = response['keyPair']['PrivateKey']

    # attach certificate to thing
    iot.attach_thing_principal(
        thingName=deviceId,
        principal=certificateArn
    )

    # create a policy for thing
    policyName = 'Policy_' + deviceId
    iot.create_policy(
        policyName=policyName,
        policyDocument=json.dumps(policyDocument)
    )

    # attach policy to certificate
    iot.attach_policy(
        policyName=policyName,
        target=certificateArn
    )

    return {
        'statusCode': 200,
        'body': {
            'certificatePem': certificatePem,
            'privateKey': privateKey
        }
    }
