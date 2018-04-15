pipeline {
    agent none
    stages {
        stage('Build') {
            agent {
                docker {
                    image 'stabletec/jekyll:latest'
                    label 'linux-docker'
                }
            }
            steps {
                sh '''
                    bundle exec jekyll build
                '''
            }
        }
    }
}